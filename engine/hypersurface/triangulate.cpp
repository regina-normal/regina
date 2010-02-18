
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
 *     each with vertices numbered 012.  Vertex 0 is always on the edge AC,
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
    struct DiscData {
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
    // relevant DiscData maps.  We don't actually need this for the
    // boundary tetrahedra, but for now we'll just do everything.
    typedef NDiscSetTetData<DiscData> TetData;
    unsigned long nTets = outer->getNumberOfTetrahedra();
    TetData** tetData = new TetData*[nTets];

    unsigned long tet;
    for (tet = 0; tet < nTets; ++tet) {
        tetData[tet] = new TetData(TODO, TODO, TODO, TODO, TODO, TODO, TODO);
    }

    // Run through normal tetrahedra, setting up DiscData maps as we go.
    unsigned long nPents = outer->getNumberOfPentachora();
    unsigned long pent;
    Dim4Pentachoron* outerPent;
    Dim4Tetrahedron* outerTet;
    NPerm5 outerTetEmb;

    int type;
    unsigned long pieceNumber;

    DiscData* discData;
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

                    discData = tetData[outerTetDisc.tetIndex]->data(
                        outerTetDisc.type, outerTetDisc.number);

                    triData = discData.data; // Only one triangle.
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
    NPerm5 roles;
    int e0, e1, f0, f1, f2; // Vertices of the pentachoron that play
                            // various roles in the location of the prism.
    for (pent = 0; pent < nPents; ++pent) {
        outerPent = outer->getPentachoron(pent);
        for (type = 0; type < 10; ++type) {
            e0 = Dim4Edge::edgeVertex[type][0];
            e1 = Dim4Edge::edgeVertex[type][1];
            f0 = Dim4Face::faceVertex[type][0];
            f1 = Dim4Face::faceVertex[type][1];
            f2 = Dim4Face::faceVertex[type][2];

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
                facet = e0;

                outerTet = outerPent->getTetrahedron(facet);
                outerTetEmb = outerPent->getTetrahedronMapping(facet);
                outerTetDisc.tetIndex = outer->getTetrahedronIndex(outerTet);
                outerTetDisc.type = outerTetEmb.preImageOf(e1);
                outerTetDisc.number = pieceNumber +
                    getTetrahedronCoord(pent, e1);

                discData = tetData[outerTetDisc.tetIndex]->data(
                    outerTetDisc.type, outerTetDisc.number);

                triData = discData.data; // Only one triangle.
                triData->map[triData->nMaps].dest = innerTet[0];
                triData->map[triData->nMaps].vertexMap = perm5to4(
                        NPerm5(f0, f1, f2, e1, e0).inverse() *
                        outerTetEmb *
                        NPerm5(3, outerTetDisc.type));
                ++triData->nMaps;

                // Tetrahedron #2:
                facet = e1;

                outerTet = outerPent->getTetrahedron(facet);
                outerTetEmb = outerPent->getTetrahedronMapping(facet);
                outerTetDisc.tetIndex = outer->getTetrahedronIndex(outerTet);
                outerTetDisc.type = outerTetEmb.preImageOf(e0);
                outerTetDisc.number = pieceNumber +
                    getTetrahedronCoord(pent, e0);

                discData = tetData[outerTetDisc.tetIndex]->data(
                    outerTetDisc.type, outerTetDisc.number);

                triData = discData.data; // Only one triangle.
                triData->map[triData->nMaps].dest = innerTet[2];
                triData->map[triData->nMaps].vertexMap = perm5to4(
                        NPerm5(f1, f2, e0, f0, e1).inverse() *
                        outerTetEmb *
                        NPerm5(3, outerTetDisc.type));
                ++triData->nMaps;

                // Now pick off the quadrilaterals that run along the
                // sides of the prism.

                // Quadrilateral #1:
                facet = f0;

                outerTet = outerPent->getTetrahedron(facet);
                outerTetEmb = outerPent->getTetrahedronMapping(facet);
                outerTetDisc.tetIndex = outer->getTetrahedronIndex(outerTet);
                outerTetDisc.type = vertexSplit
                    [outerTetEmb.preImageOf(e0)][outerTetEmb.preImageOf(e1)];
                // Quadrilaterals are numbered away from vertex 0 of the
                // outer tetrahedron.  Prisms are numbered away from
                // edge e0-e1 of the pentachoron.
                if (outerTetEmb[0] == e0 || outerTetEmb[0] == e1)
                    outerTetDisc.number = pieceNumber;
                else
                    outerTetDisc.number = getPrismCoord(pent, type)
                        + getPrismCoord(pent, Dim4Edge::edgeNumber[f1][f2])
                        - pieceNumber - 1;

                discData = tetData[outerTetDisc.tetIndex]->data(
                    outerTetDisc.type, outerTetDisc.number);

                // Map (A,B,C,D) of the outer tetrahedron to vertices of
                // the pentachoron.
                roles = outerTetEmb * NPerm5(
                    vertexSplitDefn[outerTetDisc.type][0],
                    vertexSplitDefn[outerTetDisc.type][1],
                    vertexSplitDefn[outerTetDisc.type][2],
                    vertexSplitDefn[outerTetDisc.type][3],
                    4);

                // Set things up to be correct if A,B,C,D == e0,e1,f1,f2.
                triData = discData.data; // First triangle.
                triData->map[triData->nMaps].dest = innerTet[2];
                triData->map[triData->nMaps].vertexMap = NPerm4();
                ++triData->nMaps;

                triData = discData.data + 1; // Second triangle.
                triData->map[triData->nMaps].dest = innerTet[1];
                triData->map[triData->nMaps].vertexMap = NPerm4();
                ++triData->nMaps;

                // Adjust according to the real arrangement of A,B,C,D.
                adjustMaps(
                    discData.data[0].map[discData.data[0].nMaps - 1],
                    discData.data[1].map[discData.data[1].nMaps - 1],
                    perm5to4(NPerm5(e0, e1, f1, f2, f0).inverse() * roles));


                // Eight possible cases (sigh).
                // TODO: Replace all of this with adjustMaps().

                if (e0 == roles[0]) {
                    if (f1 == roles[2]) {
                        // A,B,C,D == e0,e1,f1,f2
                        triData = discData.data; // First triangle.
                        triData->map[triData->nMaps].dest = innerTet[2];
                        triData->map[triData->nMaps].vertexMap =
                            NPerm4(0,1,2,3);
                        ++triData->nMaps;

                        triData = discData.data + 1; // Second triangle.
                        triData->map[triData->nMaps].dest = innerTet[1];
                        triData->map[triData->nMaps].vertexMap =
                            NPerm4(0,1,2,3);
                        ++triData->nMaps;
                    } else {
                        // A,B,C,D == e0,e1,f2,f1
                        // Requires a layering. TODO
                        triData = discData.data; // First triangle.
                        triData->map[triData->nMaps].dest = innerTet[TODO];
                        triData->map[triData->nMaps].vertexMap = perm5to4(
                            NPerm5(3, outerTetDisc.type));
                        ++triData->nMaps;

                        triData = discData.data + 1; // Second triangle.
                        triData->map[triData->nMaps].dest = innerTet[TODO];
                        triData->map[triData->nMaps].vertexMap = TODO;
                        ++triData->nMaps;
                    }
                } else if (e0 == roles[1]) {
                    if (f1 == roles[2]) {
                        // A,B,C,D == e1,e0,f1,f2
                        // Requires a layering. TODO
                        triData = discData.data; // First triangle.
                        triData->map[triData->nMaps].dest = innerTet[TODO];
                        triData->map[triData->nMaps].vertexMap = perm5to4(
                            NPerm5(3, outerTetDisc.type));
                        ++triData->nMaps;

                        triData = discData.data + 1; // Second triangle.
                        triData->map[triData->nMaps].dest = innerTet[TODO];
                        triData->map[triData->nMaps].vertexMap = TODO;
                        ++triData->nMaps;
                    } else {
                        // A,B,C,D == e1,e0,f2,f1
                        triData = discData.data; // First triangle.
                        triData->map[triData->nMaps].dest = innerTet[1];
                        triData->map[triData->nMaps].vertexMap =
                            NPerm4(2,1,0,3);
                        ++triData->nMaps;

                        triData = discData.data + 1; // Second triangle.
                        triData->map[triData->nMaps].dest = innerTet[2];
                        triData->map[triData->nMaps].vertexMap =
                            NPerm4(2,1,0,3);
                        ++triData->nMaps;
                    }
                } else if (e0 == roles[2]) {
                    if (f1 == roles[0]) {
                        // A,B,C,D == f1,f2,e0,e1
                        triData = discData.data; // First triangle.
                        triData->map[triData->nMaps].dest = innerTet[1];
                        triData->map[triData->nMaps].vertexMap =
                            NPerm4(0,1,2,3);
                        ++triData->nMaps;

                        triData = discData.data + 1; // Second triangle.
                        triData->map[triData->nMaps].dest = innerTet[2];
                        triData->map[triData->nMaps].vertexMap =
                            NPerm4(0,1,2,3);
                        ++triData->nMaps;
                    } else {
                        // A,B,C,D == f2,f1,e0,e1
                        // Requires a layering. TODO
                        triData = discData.data; // First triangle.
                        triData->map[triData->nMaps].dest = innerTet[TODO];
                        triData->map[triData->nMaps].vertexMap = perm5to4(
                            NPerm5(3, outerTetDisc.type));
                        ++triData->nMaps;

                        triData = discData.data + 1; // Second triangle.
                        triData->map[triData->nMaps].dest = innerTet[TODO];
                        triData->map[triData->nMaps].vertexMap = TODO;
                        ++triData->nMaps;
                    }
                } else { // e0 == roles[3]
                    if (f1 == roles[0]) {
                        // A,B,C,D == f1,f2,e1,e0
                        // Requires a layering. TODO
                        triData = discData.data; // First triangle.
                        triData->map[triData->nMaps].dest = innerTet[TODO];
                        triData->map[triData->nMaps].vertexMap = perm5to4(
                            NPerm5(3, outerTetDisc.type));
                        ++triData->nMaps;

                        triData = discData.data + 1; // Second triangle.
                        triData->map[triData->nMaps].dest = innerTet[TODO];
                        triData->map[triData->nMaps].vertexMap = TODO;
                        ++triData->nMaps;
                    } else {
                        // A,B,C,D == f2,f1,e1,e0
                        triData = discData.data; // First triangle.
                        triData->map[triData->nMaps].dest = innerTet[2];
                        triData->map[triData->nMaps].vertexMap =
                            NPerm4(2,1,0,3);
                        ++triData->nMaps;

                        triData = discData.data + 1; // Second triangle.
                        triData->map[triData->nMaps].dest = innerTet[1];
                        triData->map[triData->nMaps].vertexMap =
                            NPerm4(2,1,0,3);
                        ++triData->nMaps;
                    }
                }

                // Quadrilateral #2:
                facet = f1;

                outerTet = outerPent->getTetrahedron(facet);
                outerTetEmb = outerPent->getTetrahedronMapping(facet);
                outerTetDisc.tetIndex = outer->getTetrahedronIndex(outerTet);
                outerTetDisc.type = vertexSplit
                    [outerTetEmb.preImageOf(e0)][outerTetEmb.preImageOf(e1)];
                // Quadrilaterals are numbered away from vertex 0 of the
                // outer tetrahedron.  Prisms are numbered away from
                // edge e0-e1 of the pentachoron.
                if (outerTetEmb[0] == e0 || outerTetEmb[0] == e1)
                    outerTetDisc.number = pieceNumber;
                else
                    outerTetDisc.number = getPrismCoord(pent, type)
                        + getPrismCoord(pent, Dim4Edge::edgeNumber[f0][f2])
                        - pieceNumber - 1;

                discData = tetData[outerTetDisc.tetIndex]->data(
                    outerTetDisc.type, outerTetDisc.number);

                // Map (A,B,C,D) of the outer tetrahedron to vertices of
                // the pentachoron.
                roles = outerTetEmb * NPerm5(
                    vertexSplitDefn[outerTetDisc.type][0],
                    vertexSplitDefn[outerTetDisc.type][1],
                    vertexSplitDefn[outerTetDisc.type][2],
                    vertexSplitDefn[outerTetDisc.type][3],
                    4);

                // Set things up to be correct if A,B,C,D == e0,e1,f0,f2.
                triData = discData.data; // First triangle.
                triData->map[triData->nMaps].dest = innerTet[2];
                triData->map[triData->nMaps].vertexMap = NPerm4(0, 3);
                ++triData->nMaps;

                triData = discData.data + 1; // Second triangle.
                triData->map[triData->nMaps].dest = innerTet[0];
                triData->map[triData->nMaps].vertexMap = NPerm4(3, 0, 2, 1);
                ++triData->nMaps;

                // Adjust according to the real arrangement of A,B,C,D.
                adjustMaps(
                    discData.data[0].map[discData.data[0].nMaps - 1],
                    discData.data[1].map[discData.data[1].nMaps - 1],
                    perm5to4(NPerm5(e0, e1, f0, f2, f1).inverse() * roles));

                // Quadrilateral #3:
                facet = f2;

                outerTet = outerPent->getTetrahedron(facet);
                outerTetEmb = outerPent->getTetrahedronMapping(facet);
                outerTetDisc.tetIndex = outer->getTetrahedronIndex(outerTet);
                outerTetDisc.type = vertexSplit
                    [outerTetEmb.preImageOf(e0)][outerTetEmb.preImageOf(e1)];
                // Quadrilaterals are numbered away from vertex 0 of the
                // outer tetrahedron.  Prisms are numbered away from
                // edge e0-e1 of the pentachoron.
                if (outerTetEmb[0] == e0 || outerTetEmb[0] == e1)
                    outerTetDisc.number = pieceNumber;
                else
                    outerTetDisc.number = getPrismCoord(pent, type)
                        + getPrismCoord(pent, Dim4Edge::edgeNumber[f0][f1])
                        - pieceNumber - 1;

                discData = tetData[outerTetDisc.tetIndex]->data(
                    outerTetDisc.type, outerTetDisc.number);

                // Map (A,B,C,D) of the outer tetrahedron to vertices of
                // the pentachoron.
                roles = outerTetEmb * NPerm5(
                    vertexSplitDefn[outerTetDisc.type][0],
                    vertexSplitDefn[outerTetDisc.type][1],
                    vertexSplitDefn[outerTetDisc.type][2],
                    vertexSplitDefn[outerTetDisc.type][3],
                    4);

                // Set things up to be correct if A,B,C,D == e0,e1,f0,f1.
                triData = discData.data; // First triangle.
                triData->map[triData->nMaps].dest = innerTet[1];
                triData->map[triData->nMaps].vertexMap = NPerm4(3, 0, 1, 2);
                ++triData->nMaps;

                triData = discData.data + 1; // Second triangle.
                triData->map[triData->nMaps].dest = innerTet[0];
                triData->map[triData->nMaps].vertexMap = NPerm4(3, 0, 1, 2);
                ++triData->nMaps;

                // Adjust according to the real arrangement of A,B,C,D.
                adjustMaps(
                    discData.data[0].map[discData.data[0].nMaps - 1],
                    discData.data[1].map[discData.data[1].nMaps - 1],
                    perm5to4(NPerm5(e0, e1, f0, f1, f2).inverse() * roles));
            }
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

