
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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
#include "manifold/nlensspace.h"
#include "manifold/nsimplesurfacebundle.h"
#include "manifold/nsnappeamanifold.h"
#include "triangulation/ncomponent.h"
#include "triangulation/nedge.h"
#include "triangulation/nface.h"
#include "triangulation/nvertex.h"
#include "subcomplex/ntrivialtri.h"

namespace regina {

const int NTrivialTri::N2 = 200;
const int NTrivialTri::N3_1 = 301;
const int NTrivialTri::N3_2 = 302;
const int NTrivialTri::SPHERE_4_VERTEX = 5000;
const int NTrivialTri::SNAPPEA_M000 = 6500;
const int NTrivialTri::SNAPPEA_M001 = 6501;
const int NTrivialTri::SNAPPEA_M002 = 6502;
const int NTrivialTri::SNAPPEA_M003 = 6503;
const int NTrivialTri::SNAPPEA_M004 = 6504;

NTrivialTri* NTrivialTri::isTrivialTriangulation(const NComponent* comp) {
    // Since the triangulations are so small we can use census results
    // to recognise the triangulations by properties alone.

    if (! comp->isClosed()) {
        // We can recognise a few things from the SnapPea census, but
        // that's it for non-closed triangulations.

        // Before we do any further checks, make sure the number of
        // tetrahedra is in the supported range.
        if (comp->getNumberOfTetrahedra() > 2)
            return 0;

        // Start with property checks to see if it has a chance of being
        // in the SnapPea census at all.
        // Every edge must be valid, and every vertex link must be
        // either a torus or a Klein bottle.  Note that this implies
        // that there are no boundary faces.

        unsigned long nVertices = comp->getNumberOfVertices();
        unsigned long nEdges = comp->getNumberOfEdges();
        unsigned long i;
        int link;
        for (i = 0; i < nVertices; i++) {
            link = comp->getVertex(i)->getLink();
            if (link != NVertex::TORUS && link != NVertex::KLEIN_BOTTLE)
                return 0;
        }
        for (i = 0; i < nEdges; i++)
            if (! comp->getEdge(i)->isValid())
                return 0;

        // Now search for SnapPea triangulations m000 -- m004.

        if (comp->getNumberOfTetrahedra() == 1) {
            // At this point it must be m000, since there are no others
            // that fit these constraints.  But test orientability
            // anyway just to be safe.
            if (comp->isOrientable())
                return 0;
            return new NTrivialTri(SNAPPEA_M000);
        } else if (comp->getNumberOfTetrahedra() == 2) {
            if (comp->isOrientable()) {
                // Orientable.  Looking for m003 or m004.
                if (comp->getNumberOfVertices() != 1)
                    return 0;
                if (comp->getNumberOfEdges() != 2)
                    return 0;
                if (comp->getEdge(0)->getNumberOfEmbeddings() != 6 ||
                        comp->getEdge(1)->getNumberOfEmbeddings() != 6)
                    return 0;

                // Now we know it's either m003 or m004.  We distinguish
                // between them by face types, since all of m003's faces
                // are Mobius bands and all of m004's faces are horns.
                if (comp->getFace(0)->getType() == NFace::MOBIUS)
                    return new NTrivialTri(SNAPPEA_M003);
                else
                    return new NTrivialTri(SNAPPEA_M004);
            } else {
                // Non-orientable.  Looking for m001 or m002.
                if (comp->getNumberOfVertices() == 1) {
                    // Looking for m001.
                    if (comp->getNumberOfEdges() != 2)
                        return 0;
                    if (! ((comp->getEdge(0)->getNumberOfEmbeddings() == 4 &&
                            comp->getEdge(1)->getNumberOfEmbeddings() == 8) ||
                           (comp->getEdge(0)->getNumberOfEmbeddings() == 8 &&
                            comp->getEdge(1)->getNumberOfEmbeddings() == 4)))
                        return 0;
                    // Census says it's m001 if no face forms a dunce hat.
                    for (int i = 0; i < 4; i++)
                        if (comp->getFace(i)->getType() == NFace::DUNCEHAT)
                            return 0;
                    return new NTrivialTri(SNAPPEA_M001);
                } else if (comp->getNumberOfVertices() == 2) {
                    // Looking for m002.
                    if (comp->getNumberOfEdges() != 2)
                        return 0;
                    if (comp->getEdge(0)->getNumberOfEmbeddings() != 6 ||
                            comp->getEdge(1)->getNumberOfEmbeddings() != 6)
                        return 0;
                    // Census says it's m002 if some face forms a dunce hat.
                    for (int i = 0; i < 4; i++)
                        if (comp->getFace(i)->getType() == NFace::DUNCEHAT)
                            return new NTrivialTri(SNAPPEA_M002);
                    return 0;
                }
            }
        }

        // Guess we don't know it after all.
        return 0;
    }

    // Otherwise we're looking at a closed triangulation.

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
        if (comp->isOrientable()) {
            if (comp->getNumberOfVertices() == 4) {
                // There's only one closed valid two-tetrahedron
                // four-vertex orientable triangulation.
                return new NTrivialTri(SPHERE_4_VERTEX);
            }
        } else {
            // There's only one closed valid two-tetrahedron non-orientable
            // triangulation.
            return new NTrivialTri(N2);
        }
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
    if (type == SPHERE_4_VERTEX)
        return new NLensSpace(1, 0);
    else if (type == N2)
        return new NSimpleSurfaceBundle(NSimpleSurfaceBundle::S2xS1_TWISTED);
    else if (type == N3_1 || type == N3_2)
        return new NSimpleSurfaceBundle(NSimpleSurfaceBundle::RP2xS1);
    else if (type == SNAPPEA_M000)
        return new NSnapPeaCensusManifold(NSnapPeaCensusManifold::SEC_5, 0);
    else if (type == SNAPPEA_M001)
        return new NSnapPeaCensusManifold(NSnapPeaCensusManifold::SEC_5, 1);
    else if (type == SNAPPEA_M002)
        return new NSnapPeaCensusManifold(NSnapPeaCensusManifold::SEC_5, 2);
    else if (type == SNAPPEA_M003)
        return new NSnapPeaCensusManifold(NSnapPeaCensusManifold::SEC_5, 3);
    else if (type == SNAPPEA_M004)
        return new NSnapPeaCensusManifold(NSnapPeaCensusManifold::SEC_5, 4);
    return 0;
}

NAbelianGroup* NTrivialTri::getHomologyH1() const {
    NAbelianGroup* ans = new NAbelianGroup();

    if (type == N2)
        ans->addRank();
    else if (type == N3_1 || type == N3_2) {
        ans->addRank();
        ans->addTorsionElement(2);
    } else if (type == SNAPPEA_M000 || type == SNAPPEA_M004)
        ans->addRank();
    else if (type == SNAPPEA_M001 || type == SNAPPEA_M002) {
        ans->addRank();
        ans->addTorsionElement(2);
    } else if (type == SNAPPEA_M003) {
        ans->addRank();
        ans->addTorsionElement(5);
    }

    return ans;
}

inline std::ostream& NTrivialTri::writeName(std::ostream& out) const {
    if (type == SPHERE_4_VERTEX)
        out << "S3 (4-vtx)";
    else if (type == N2)
        out << "N(2)";
    else if (type == N3_1)
        out << "N(3,1)";
    else if (type == N3_2)
        out << "N(3,2)";
    else if (type == SNAPPEA_M000)
        out << "SnapPea m000";
    else if (type == SNAPPEA_M001)
        out << "SnapPea m001";
    else if (type == SNAPPEA_M002)
        out << "SnapPea m002";
    else if (type == SNAPPEA_M003)
        out << "SnapPea m003";
    else if (type == SNAPPEA_M004)
        out << "SnapPea m004";
    return out;
}
inline std::ostream& NTrivialTri::writeTeXName(std::ostream& out) const {
    if (type == SPHERE_4_VERTEX)
        out << "$S^3_{v=4}$";
    else if (type == N2)
        out << "$N_{2}$";
    else if (type == N3_1)
        out << "$N_{3,1}$";
    else if (type == N3_2)
        out << "$N_{3,2}$";
    else if (type == SNAPPEA_M000)
        out << "$m_{000}$";
    else if (type == SNAPPEA_M001)
        out << "$m_{001}$";
    else if (type == SNAPPEA_M002)
        out << "$m_{002}$";
    else if (type == SNAPPEA_M003)
        out << "$m_{003}$";
    else if (type == SNAPPEA_M004)
        out << "$m_{004}$";
    return out;
}
inline void NTrivialTri::writeTextLong(std::ostream& out) const {
    if (type == SPHERE_4_VERTEX)
        out << "Two-tetrahedron four-vertex 3-sphere";
    else if (type == N2)
        out << "Non-orientable triangulation N(2)";
    else if (type == N3_1)
        out << "Non-orientable triangulation N(3,1)";
    else if (type == N3_2)
        out << "Non-orientable triangulation N(3,2)";
    else if (type == SNAPPEA_M000)
        out << "SnapPea triangulation m000";
    else if (type == SNAPPEA_M001)
        out << "SnapPea triangulation m001";
    else if (type == SNAPPEA_M002)
        out << "SnapPea triangulation m002";
    else if (type == SNAPPEA_M003)
        out << "SnapPea triangulation m003";
    else if (type == SNAPPEA_M004)
        out << "SnapPea triangulation m004";
}

} // namespace regina

