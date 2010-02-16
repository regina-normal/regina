
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

#include "dim4/dim4edge.h"
#include "dim4/dim4face.h"
#include "dim4/dim4triangulation.h"
#include "hypersurface/nnormalhypersurface.h"
#include "surfaces/ndisc.h"
#include "triangulation/ntriangulation.h"

namespace regina {

/**
 * This file contains the implementation for NNormalHypersurface::triangulate().
 *
 * As part of the triangulation process, we track the following information:
 *
 * - Normal tetrahedron/prism pieces in pentachora are bounded by normal
 *   triangles/quadrilaterals in tetrahedra.
 *
 *   + For a triangle surrounding vertex V of a tetrahedron, the
 *     triangle vertices are numbered 012.  This is done so that vertex i of
 *     the triangle lies on the edge joining vertices V,i of the tetrahedron
 *     (or, if i=V, then joining vertices V,3 of the tetrahedron).
 *
 *   + For a quadrilateral separating tetrahedron vertices A,B from C,D
 *     (A < B, C < D, A < C), we divide the quadrilateral into two triangles,
 *     each with vertices numbered 012.  Vertex 0 is always on the edge AD,
 *     vertex 2 is always on the edge BD, and the edge between these
 *     vertices is common to both triangles.  The first triangle is the
 *     one that touches edge AD, and the second triangle is the one that
 *     touches edge BC.
 *
 * - For each of the labelled triangles described above, we store maps
 *   from vertices 012 of the triangle to vertices of a tetrahedron in a
 *   triangulated normal piece (we always map 4 -> 4).  This information
 *   is kept in a TriangleMap struct.  Each triangle has one or two associated
 *   maps, according to whether it lies on the boundary of one or two normal
 *   pieces; these are stored together in a TriangleData struct.
 *   Since normal quadrilaterals decompose into two triangles, we store
 *   the maps for a single normal disc in a DiscTriangleData struct.
 */

// ------------------------------------------------------------------------
// Supporting classes for triangulate()
// ------------------------------------------------------------------------

namespace {
    /**
     * Maps a triangle on the boundary of a triangulated normal piece to a
     * tetrahedron of that normal piece.
     */
    struct TriangleMap {
        NTetrahedron* dest;
        NPerm4 vertexMap;
    };

    /**
     * Stores all TriangleMap maps for a given triangle on the boundary
     * of one or more triangulated normal pieces.  Note that there are
     * only ever one or two such maps.
     */
    struct TriangleData {
        TriangleMap map[2];
        int nMaps;

        TriangleData() : nMaps(0) {}
    };

    /**
     * Contains TriangleData data for all triangles that together form a
     * single normal triangle/quadrilateral within a tetrahedron of the
     * surrounding 4-manifold triangulation.  There is one such triangle
     * for a triangular normal disc, and there are two such triangles
     * for a quadrilateral normal disc.
     */
    struct PieceData {
        TriangleData data[2];
    };
}

// ------------------------------------------------------------------------
// Implementation of triangulate()
// ------------------------------------------------------------------------

NTriangulation* NNormalHypersurface::triangulate() const {
    const Dim4Triangulation* outer = getTriangulation();
    NTriangulation* inner = new NTriangulation();

    // Get rid of an empty *outer* triangulation now.
    if (outer->getNumberOfPentachora() == 0)
        return inner;

    // Set up one NDiscSetTetData for each tetrahedron, storing the
    // relevant PieceData maps.  We don't actually need this for the
    // boundary tetrahedra, but for now we'll just do everything.
    typedef NDiscSetTetData<PieceData> TetData;
    unsigned long nTets = outer->getNumberOfTetrahedra();
    TetData** tetData = new TetData*[nTets];

    unsigned long tet;
    for (tet = 0; tet < nTets; ++tet) {
        tetData[tet] = new TetData(TODO, TODO, TODO, TODO, TODO, TODO, TODO);
    }

    // Run through normal tetrahedra, setting up PieceData maps as we go.
    unsigned long nPents = outer->getNumberOfPentachora();
    unsigned long pent;
    Dim4Pentachoron* outerPent;
    Dim4Tetrahedron* outerTet;
    NPerm5 outerTetEmb;

    int type;
    unsigned long pieceNumber;

    PieceData* pieceData;
    TriangleData* triData;
    NDiscSpec outerTetDisc;

    NTetrahedron** innerTet[3];
    int facet;

    for (pent = 0; pent < nPents; ++pent) {
        outerPent = outer->getPentachoron(pent);
        for (type = 0; type < 5; ++type)
            for (pieceNumber = 0; pieceNumber < getTetrahedronCoord(pent, type);
                    ++pieceNumber) {
                // Create a new tetrahedron for the final 3-manifold
                // triangulation.
                innerTet[0] = new NTetrahedron();
                inner->insertTetrahedron(innerTet[0]);

                for (facet = 0; facet < 5; ++facet) {
                    if (facet == type)
                        continue;

                    // See how this tetrahedron meets facet i of the
                    // pentachoron.
                    outerTet = outerPent->getTetrahedron(facet);
                    outerTetEmb = outerPent->getTetrahedronMapping(facet);

                    outerTetDisc.tetIndex = outer->tetrahedronIndex(outerTet);
                    outerTetDisc.type = outerTetEmb.preImageOf(type);
                    outerTetDisc.number = pieceNumber;

                    pieceData = tetData[outerTetDisc.tetIndex]->data(
                        outerTetDisc.type, outerTetDisc.number);

                    triData = pieceData.data; // Only one triangle.
                    triData->map[triData->nMaps].dest = innerTet[0];
                    triData->map[triData->nMaps].vertexMap = perm5to4(
                        NPerm5(4, type) *
                        outerTetEmb *
                        NPerm5(3, outerTetDisc.type) *
                        NPerm5(3, 4));
                    ++triData->nMaps;
                }
            }
    }

    // Similarly for normal prisms.
    for (pent = 0; pent < nPents; ++pent) {
        outerPent = outer->getPentachoron(pent);
        for (type = 0; type < 10; ++type)
            for (pieceNumber = 0; pieceNumber < getPrismCoord(pent, type);
                    ++pieceNumber) {
                // Triangulate the normal prism with three tetrahedra.
                innerTet[0] = new NTetrahedron();
                innerTet[1] = new NTetrahedron();
                innerTet[2] = new NTetrahedron();
                innerTet[0]->joinTo(innerTet[1], 0, NPerm4());
                innerTet[2]->joinTo(innerTet[1], 1, NPerm4());
                inner->insertTetrahedron(innerTet[0]);
                inner->insertTetrahedron(innerTet[1]);
                inner->insertTetrahedron(innerTet[2]);

                // First pick off the triangles at the ends of the prism.

                // Tetrahedron #1:
                facet = Dim4Edge::edgeVertex[type][0];

                outerTet = outerPent->getTetrahedron(facet);
                outerTetEmb = outerPent->getTetrahedronMapping(facet);
                outerTetDisc.tetIndex = outer->getTetrahedronIndex(outerTet);
                outerTetDisc.type = outerTetEmb.preImageOf(
                    Dim4Edge::edgeVertex[type][1];
                outerTetDisc.number = pieceNumber +
                    getTetrahedronCoord(pent, Dim4Edge::edgeVertex[type][1]);

                pieceData = tetData[outerTetDisc.tetIndex]->data(
                    outerTetDisc.type, outerTetDisc.number);

                triData = pieceData.data; // Only one triangle.
                triData->map[triData->nMaps].dest = innerTet[0];
                triData->map[triData->nMaps].vertexMap = perm5to4(
                        NPerm5(
                            Dim4Face::faceVertex[type][0],
                            Dim4Face::faceVertex[type][1],
                            Dim4Face::faceVertex[type][2],
                            Dim4Edge::edgeVertex[type][1],
                            Dim4Edge::edgeVertex[type][0]).inverse() *
                        outerTetEmb *
                        NPerm5(3, outerTetDisc.type));
                ++triData->nMaps;

                // Tetrahedron #2:
                facet = Dim4Edge::edgeVertex[type][1];

                outerTet = outerPent->getTetrahedron(facet);
                outerTetEmb = outerPent->getTetrahedronMapping(facet);
                outerTetDisc.tetIndex = outer->getTetrahedronIndex(outerTet);
                outerTetDisc.type = outerTetEmb.preImageOf(
                    Dim4Edge::edgeVertex[type][0];
                outerTetDisc.number = pieceNumber +
                    getTetrahedronCoord(pent, Dim4Edge::edgeVertex[type][0]);

                pieceData = tetData[outerTetDisc.tetIndex]->data(
                    outerTetDisc.type, outerTetDisc.number);

                triData = pieceData.data; // Only one triangle.
                triData->map[triData->nMaps].dest = innerTet[2];
                triData->map[triData->nMaps].vertexMap = perm5to4(
                        NPerm5(
                            Dim4Face::faceVertex[type][0],
                            Dim4Face::faceVertex[type][1],
                            Dim4Face::faceVertex[type][2],
                            Dim4Edge::edgeVertex[type][0],
                            Dim4Edge::edgeVertex[type][1]).inverse() *
                        outerTetEmb *
                        NPerm5(3, outerTetDisc.type));
                ++triData->nMaps;

                // Now pick off the quadrilaterals that run along the
                // sides of the prism.

                // Quadrilateral #1:
                facet = Dim4Face::faceVertex[type][0];

                outerTet = outerPent->getTetrahedron(facet);
                outerTetEmb = outerPent->getTetrahedronMapping(facet);
                outerTetDisc.tetIndex = outer->getTetrahedronIndex(outerTet);
                outerTetDisc.type = TODO;
                outerTetDisc.number = TODO;

                pieceData = tetData[outerTetDisc.tetIndex]->data(
                    outerTetDisc.type, outerTetDisc.number);

                triData = pieceData.data; // First triangle.
                triData->map[triData->nMaps].dest = innerTet[TODO];
                triData->map[triData->nMaps].vertexMap = TODO;
                ++triData->nMaps;

                triData = pieceData.data; // Second triangle.
                triData->map[triData->nMaps].dest = innerTet[TODO];
                triData->map[triData->nMaps].vertexMap = TODO;
                ++triData->nMaps;

                // Quadrilateral #2:
                facet = Dim4Face::faceVertex[type][1];

                outerTet = outerPent->getTetrahedron(facet);
                outerTetEmb = outerPent->getTetrahedronMapping(facet);
                outerTetDisc.tetIndex = outer->getTetrahedronIndex(outerTet);
                outerTetDisc.type = TODO;
                outerTetDisc.number = TODO;

                pieceData = tetData[outerTetDisc.tetIndex]->data(
                    outerTetDisc.type, outerTetDisc.number);

                triData = pieceData.data; // First triangle.
                triData->map[triData->nMaps].dest = innerTet[TODO];
                triData->map[triData->nMaps].vertexMap = TODO;
                ++triData->nMaps;

                triData = pieceData.data; // Second triangle.
                triData->map[triData->nMaps].dest = innerTet[TODO];
                triData->map[triData->nMaps].vertexMap = TODO;
                ++triData->nMaps;

                // Quadrilateral #3:
                facet = Dim4Face::faceVertex[type][2];

                outerTet = outerPent->getTetrahedron(facet);
                outerTetEmb = outerPent->getTetrahedronMapping(facet);
                outerTetDisc.tetIndex = outer->getTetrahedronIndex(outerTet);
                outerTetDisc.type = TODO;
                outerTetDisc.number = TODO;

                pieceData = tetData[outerTetDisc.tetIndex]->data(
                    outerTetDisc.type, outerTetDisc.number);

                triData = pieceData.data; // First triangle.
                triData->map[triData->nMaps].dest = innerTet[TODO];
                triData->map[triData->nMaps].vertexMap = TODO;
                ++triData->nMaps;

                triData = pieceData.data; // Second triangle.
                triData->map[triData->nMaps].dest = innerTet[TODO];
                triData->map[triData->nMaps].vertexMap = TODO;
                ++triData->nMaps;
            }
    }

    // TODO: Run through individual triangles from normal
    // triangles/quadrilaterals, and glue together tetrahedra from the
    // 3-dimensional normal pieces on each side.

    // And... we're done!
    // Clean up and go home.
    for (tet = 0; tet < nTets; ++tet)
        delete tetData[tet];
    delete[] tetData;

    inner->gluingsHaveChanged();
    return inner;
}

} // namespace regina

