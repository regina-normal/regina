
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include <cassert>
#include "hypersurface/normalhypersurface.h"
#include "surfaces/disc.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"

namespace regina {

/**
 * This file contains the implementation for NormalHypersurface::triangulate().
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
        Tetrahedron<3>* dest;
        Perm<4> vertexMap;
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

    /**
     * Fixes deliberate errors when setting up the triangle maps for a
     * quadrilateral normal disc.
     *
     * We create the maps assuming a given ordering of vertices A,B,C,D
     * for the surrounding tetrahedron.  This routine fixes things in
     * the case where that ordering was wrong.
     *
     * The permutation \a err maps the real vertices (A,B,C,D) to what
     * where we \e thought those vertices were.  Specifically, if
     * err[x] == y, this means that the real vertex x has been glued
     * into the position that really belongs to vertex y.
     */
    void adjustQuadMaps(TriangleMap& map0, TriangleMap& map1,
            Perm<4> err, Triangulation<3>* tri) {
        if (err.isIdentity()) {
            // The mappings are already correct.
            return;
        }

        Tetrahedron<3>* oldd0 = map0.dest;
        Tetrahedron<3>* oldd1 = map1.dest;
        Perm<4> oldv0 = map0.vertexMap;
        Perm<4> oldv1 = map1.vertexMap;

        if (err == Perm<4>(1,0,3,2)) {
            // Rotate by 180 degrees.
            map0.dest = oldd1;
            map1.dest = oldd0;
            map0.vertexMap = oldv1 * Perm<4>(0,2);
            map1.vertexMap = oldv0 * Perm<4>(0,2);
            return;
        }

        if (err == Perm<4>(1,0)) {
            // Reflect A,B.
            // This requires a layering.
            Tetrahedron<3>* tet = tri->newTetrahedron();
            tet->join(1, oldd0, oldv0 * Perm<4>(1,3));
            tet->join(3, oldd1, oldv1);

            map0.dest = map1.dest = tet;
            map0.vertexMap = Perm<4>(1,2,3,0);
            map1.vertexMap = Perm<4>(1,0,3,2);
            return;
        }

        if (err == Perm<4>(2,3)) {
            // Reflect C,D.
            // This again requires a layering.
            Tetrahedron<3>* tet = tri->newTetrahedron();
            tet->join(1, oldd0, oldv0 * Perm<4>(1,3));
            tet->join(3, oldd1, oldv1);

            map0.dest = map1.dest = tet;
            map0.vertexMap = Perm<4>(3,0,1,2);
            map1.vertexMap = Perm<4>(3,2,1,0);
            return;
        }

        if (err == Perm<4>(2,3,0,1)) {
            // Switch (A,B) with (C,D).
            // This is a reflection across the diagonal edge.
            map0.dest = oldd1;
            map1.dest = oldd0;
            map0.vertexMap = oldv1;
            map1.vertexMap = oldv0;
            return;
        }

        if (err == Perm<4>(3,2,1,0)) {
            // Switch (A,B) with (C,D) and reflect both pairs.
            // This is a reflection across the off-diagonal.
            map0.vertexMap = oldv0 * Perm<4>(0,2);
            map1.vertexMap = oldv1 * Perm<4>(0,2);
            return;
        }

        if (err == Perm<4>(2,3,1,0)) {
            // Rotate the quadrilateral.
            // This requires a layering.
            Tetrahedron<3>* tet = tri->newTetrahedron();
            tet->join(1, oldd0, oldv0 * Perm<4>(1,3));
            tet->join(3, oldd1, oldv1);

            map0.dest = map1.dest = tet;
            map0.vertexMap = Perm<4>(1,0,3,2);
            map1.vertexMap = Perm<4>(1,2,3,0);
            return;
        }

        if (err == Perm<4>(3,2,0,1)) {
            // Rotate the quadrilateral in the other direction.
            // This also requires a layering.
            Tetrahedron<3>* tet = tri->newTetrahedron();
            tet->join(1, oldd0, oldv0 * Perm<4>(1,3));
            tet->join(3, oldd1, oldv1);

            map0.dest = map1.dest = tet;
            map0.vertexMap = Perm<4>(3,2,1,0);
            map1.vertexMap = Perm<4>(3,0,1,2);
            return;
        }

        // We should never fall through to here.
        assert(false);
    }
}

// ------------------------------------------------------------------------
// Implementation of triangulate()
// ------------------------------------------------------------------------

Triangulation<3>* NormalHypersurface::triangulate() const {
    const Triangulation<4>* outer = triangulation();
    Triangulation<3>* inner = new Triangulation<3>();

    // Get rid of an empty *outer* triangulation now.
    if (outer->isEmpty())
        return inner;

    // Set up one DiscSetTetData for each tetrahedron, storing the
    // relevant DiscData maps.  We don't actually need this for the
    // boundary tetrahedra, but for now we'll just do everything.
    typedef DiscSetTetData<DiscData> TetData;
    size_t nTets = outer->countTetrahedra();
    TetData** tetData = new TetData*[nTets];

    const Pentachoron<4>* outerPent;
    Tetrahedron<4>* outerTet;
    Perm<5> outerTetEmb;

    size_t tet;
    size_t pent;
    for (tet = 0; tet < nTets; ++tet) {
        outerTet = outer->tetrahedron(tet);
        outerPent = outerTet->front().pentachoron();
        outerTetEmb = outerTet->front().vertices();
        pent = outerPent->index();

        tetData[tet] = new TetData(
            tetrahedra(pent, outerTetEmb[0]).longValue() +
                prisms(pent,
                    Edge<4>::edgeNumber[outerTetEmb[0]][outerTetEmb[4]]).
                    longValue(),
            tetrahedra(pent, outerTetEmb[1]).longValue() +
                prisms(pent,
                    Edge<4>::edgeNumber[outerTetEmb[1]][outerTetEmb[4]]).
                    longValue(),
            tetrahedra(pent, outerTetEmb[2]).longValue() +
                prisms(pent,
                    Edge<4>::edgeNumber[outerTetEmb[2]][outerTetEmb[4]]).
                    longValue(),
            tetrahedra(pent, outerTetEmb[3]).longValue() +
                prisms(pent,
                    Edge<4>::edgeNumber[outerTetEmb[3]][outerTetEmb[4]]).
                    longValue(),
            prisms(pent, Edge<4>::edgeNumber
                    [outerTetEmb[quadDefn[0][0]]]
                    [outerTetEmb[quadDefn[0][1]]]).longValue() +
                prisms(pent, Edge<4>::edgeNumber
                    [outerTetEmb[quadDefn[0][2]]]
                    [outerTetEmb[quadDefn[0][3]]]).longValue(),
            prisms(pent, Edge<4>::edgeNumber
                    [outerTetEmb[quadDefn[1][0]]]
                    [outerTetEmb[quadDefn[1][1]]]).longValue() +
                prisms(pent, Edge<4>::edgeNumber
                    [outerTetEmb[quadDefn[1][2]]]
                    [outerTetEmb[quadDefn[1][3]]]).longValue(),
            prisms(pent, Edge<4>::edgeNumber
                    [outerTetEmb[quadDefn[2][0]]]
                    [outerTetEmb[quadDefn[2][1]]]).longValue() +
                prisms(pent, Edge<4>::edgeNumber
                    [outerTetEmb[quadDefn[2][2]]]
                    [outerTetEmb[quadDefn[2][3]]]).longValue());
    }

    // Run through normal tetrahedra, setting up DiscData maps as we go.
    size_t nPents = outer->size();

    int type;
    unsigned long pieceNumber;

    DiscData* discData;
    TriangleData* triData;
    DiscSpec outerTetDisc;

    Tetrahedron<3>* innerTet[3];
    int facet;

    for (pent = 0; pent < nPents; ++pent) {
        outerPent = outer->pentachoron(pent);
        for (type = 0; type < 5; ++type)
            for (pieceNumber = 0;
                    pieceNumber < tetrahedra(pent, type).longValue();
                    ++pieceNumber) {
                // Create a new tetrahedron for the final 3-manifold
                // triangulation.
                innerTet[0] = inner->newTetrahedron();

                for (facet = 0; facet < 5; ++facet) {
                    if (facet == type)
                        continue;

                    // See how this tetrahedron meets facet i of the
                    // pentachoron.
                    outerTet = outerPent->tetrahedron(facet);
                    outerTetEmb = outerPent->tetrahedronMapping(facet);

                    outerTetDisc.tetIndex = outerTet->index();
                    outerTetDisc.type = outerTetEmb.preImageOf(type);
                    outerTetDisc.number = pieceNumber;

                    discData = &tetData[outerTetDisc.tetIndex]->data(
                        outerTetDisc.type, outerTetDisc.number);

                    triData = discData->data; // Only one triangle.
                    triData->map[triData->nMaps].dest = innerTet[0];
                    triData->map[triData->nMaps].vertexMap = Perm<4>::contract(
                        Perm<5>(4, type) *
                        outerTetEmb *
                        Perm<5>(3, outerTetDisc.type) *
                        Perm<5>(3, 4));
                    ++triData->nMaps;
                    assert(triData->nMaps <= 2);
                }
            }
    }

    // Similarly for normal prisms.
    Perm<5> roles;
    int e0, e1, f0, f1, f2; // Vertices of the pentachoron that play
                            // various roles in the location of the prism.
    for (pent = 0; pent < nPents; ++pent) {
        outerPent = outer->pentachoron(pent);
        for (type = 0; type < 10; ++type) {
            e0 = Edge<4>::edgeVertex[type][0];
            e1 = Edge<4>::edgeVertex[type][1];
            f0 = Triangle<4>::triangleVertex[type][0];
            f1 = Triangle<4>::triangleVertex[type][1];
            f2 = Triangle<4>::triangleVertex[type][2];

            for (pieceNumber = 0; pieceNumber < prisms(pent, type).longValue();
                    ++pieceNumber) {
                // Triangulate the normal prism with three tetrahedra.
                innerTet[0] = inner->newTetrahedron();
                innerTet[1] = inner->newTetrahedron();
                innerTet[2] = inner->newTetrahedron();
                innerTet[0]->join(0, innerTet[1], Perm<4>());
                innerTet[2]->join(1, innerTet[1], Perm<4>());

                // First pick off the triangles at the ends of the prism.

                // Tetrahedron #1:
                facet = e0;

                outerTet = outerPent->tetrahedron(facet);
                outerTetEmb = outerPent->tetrahedronMapping(facet);
                outerTetDisc.tetIndex = outerTet->index();
                outerTetDisc.type = outerTetEmb.preImageOf(e1);
                outerTetDisc.number = pieceNumber +
                    tetrahedra(pent, e1).longValue();

                discData = &tetData[outerTetDisc.tetIndex]->data(
                    outerTetDisc.type, outerTetDisc.number);

                triData = discData->data; // Only one triangle.
                triData->map[triData->nMaps].dest = innerTet[0];
                triData->map[triData->nMaps].vertexMap = Perm<4>::contract(
                        Perm<5>(f0, f1, f2, e1, e0).inverse() *
                        outerTetEmb *
                        Perm<5>(3, outerTetDisc.type));
                ++triData->nMaps;
                assert(triData->nMaps <= 2);

                // Tetrahedron #2:
                facet = e1;

                outerTet = outerPent->tetrahedron(facet);
                outerTetEmb = outerPent->tetrahedronMapping(facet);
                outerTetDisc.tetIndex = outerTet->index();
                outerTetDisc.type = outerTetEmb.preImageOf(e0);
                outerTetDisc.number = pieceNumber +
                    tetrahedra(pent, e0).longValue();

                discData = &tetData[outerTetDisc.tetIndex]->data(
                    outerTetDisc.type, outerTetDisc.number);

                triData = discData->data; // Only one triangle.
                triData->map[triData->nMaps].dest = innerTet[2];
                triData->map[triData->nMaps].vertexMap = Perm<4>::contract(
                        Perm<5>(f1, f2, e0, f0, e1).inverse() *
                        outerTetEmb *
                        Perm<5>(3, outerTetDisc.type));
                ++triData->nMaps;
                assert(triData->nMaps <= 2);

                // Now pick off the quadrilaterals that run along the
                // sides of the prism.

                // Quadrilateral #1:
                facet = f0;

                outerTet = outerPent->tetrahedron(facet);
                outerTetEmb = outerPent->tetrahedronMapping(facet);
                outerTetDisc.tetIndex = outerTet->index();
                outerTetDisc.type = 4 + quadSeparating
                    [outerTetEmb.preImageOf(e0)][outerTetEmb.preImageOf(e1)];
                // Quadrilaterals are numbered away from vertex 0 of the
                // outer tetrahedron.  Prisms are numbered away from
                // edge e0-e1 of the pentachoron.
                if (outerTetEmb[0] == e0 || outerTetEmb[0] == e1)
                    outerTetDisc.number = pieceNumber;
                else
                    outerTetDisc.number = prisms(pent, type).longValue()
                        + prisms(pent, Edge<4>::edgeNumber[f1][f2])
                            .longValue()
                        - pieceNumber - 1;

                discData = &tetData[outerTetDisc.tetIndex]->data(
                    outerTetDisc.type, outerTetDisc.number);

                // Map (A,B,C,D) of the outer tetrahedron to vertices of
                // the pentachoron.
                roles = outerTetEmb * Perm<5>(
                    quadDefn[outerTetDisc.type - 4][0],
                    quadDefn[outerTetDisc.type - 4][1],
                    quadDefn[outerTetDisc.type - 4][2],
                    quadDefn[outerTetDisc.type - 4][3],
                    4);

                // Set things up to be correct if A,B,C,D == e0,e1,f1,f2.
                triData = discData->data; // First triangle.
                triData->map[triData->nMaps].dest = innerTet[2];
                triData->map[triData->nMaps].vertexMap = Perm<4>();
                ++triData->nMaps;
                assert(triData->nMaps <= 2);

                triData = discData->data + 1; // Second triangle.
                triData->map[triData->nMaps].dest = innerTet[1];
                triData->map[triData->nMaps].vertexMap = Perm<4>();
                ++triData->nMaps;
                assert(triData->nMaps <= 2);

                // Adjust according to the real arrangement of A,B,C,D.
                adjustQuadMaps(
                    discData->data[0].map[discData->data[0].nMaps - 1],
                    discData->data[1].map[discData->data[1].nMaps - 1],
                    Perm<4>::contract(
                        Perm<5>(e0, e1, f1, f2, f0).inverse() * roles),
                    inner);

                // Quadrilateral #2:
                facet = f1;

                outerTet = outerPent->tetrahedron(facet);
                outerTetEmb = outerPent->tetrahedronMapping(facet);
                outerTetDisc.tetIndex = outerTet->index();
                outerTetDisc.type = 4 + quadSeparating
                    [outerTetEmb.preImageOf(e0)][outerTetEmb.preImageOf(e1)];
                // Quadrilaterals are numbered away from vertex 0 of the
                // outer tetrahedron.  Prisms are numbered away from
                // edge e0-e1 of the pentachoron.
                if (outerTetEmb[0] == e0 || outerTetEmb[0] == e1)
                    outerTetDisc.number = pieceNumber;
                else
                    outerTetDisc.number = prisms(pent, type).longValue()
                        + prisms(pent, Edge<4>::edgeNumber[f0][f2]).longValue()
                        - pieceNumber - 1;

                discData = &tetData[outerTetDisc.tetIndex]->data(
                    outerTetDisc.type, outerTetDisc.number);

                // Map (A,B,C,D) of the outer tetrahedron to vertices of
                // the pentachoron.
                roles = outerTetEmb * Perm<5>(
                    quadDefn[outerTetDisc.type - 4][0],
                    quadDefn[outerTetDisc.type - 4][1],
                    quadDefn[outerTetDisc.type - 4][2],
                    quadDefn[outerTetDisc.type - 4][3],
                    4);

                // Set things up to be correct if A,B,C,D == e0,e1,f0,f2.
                triData = discData->data; // First triangle.
                triData->map[triData->nMaps].dest = innerTet[2];
                triData->map[triData->nMaps].vertexMap = Perm<4>(0, 3);
                ++triData->nMaps;
                assert(triData->nMaps <= 2);

                triData = discData->data + 1; // Second triangle.
                triData->map[triData->nMaps].dest = innerTet[0];
                triData->map[triData->nMaps].vertexMap = Perm<4>(3, 0, 2, 1);
                ++triData->nMaps;
                assert(triData->nMaps <= 2);

                // Adjust according to the real arrangement of A,B,C,D.
                adjustQuadMaps(
                    discData->data[0].map[discData->data[0].nMaps - 1],
                    discData->data[1].map[discData->data[1].nMaps - 1],
                    Perm<4>::contract(
                        Perm<5>(e0, e1, f0, f2, f1).inverse() * roles),
                    inner);

                // Quadrilateral #3:
                facet = f2;

                outerTet = outerPent->tetrahedron(facet);
                outerTetEmb = outerPent->tetrahedronMapping(facet);
                outerTetDisc.tetIndex = outerTet->index();
                outerTetDisc.type = 4 + quadSeparating
                    [outerTetEmb.preImageOf(e0)][outerTetEmb.preImageOf(e1)];
                // Quadrilaterals are numbered away from vertex 0 of the
                // outer tetrahedron.  Prisms are numbered away from
                // edge e0-e1 of the pentachoron.
                if (outerTetEmb[0] == e0 || outerTetEmb[0] == e1)
                    outerTetDisc.number = pieceNumber;
                else
                    outerTetDisc.number = prisms(pent, type).longValue()
                        + prisms(pent, Edge<4>::edgeNumber[f0][f1])
                            .longValue()
                        - pieceNumber - 1;

                discData = &tetData[outerTetDisc.tetIndex]->data(
                    outerTetDisc.type, outerTetDisc.number);

                // Map (A,B,C,D) of the outer tetrahedron to vertices of
                // the pentachoron.
                roles = outerTetEmb * Perm<5>(
                    quadDefn[outerTetDisc.type - 4][0],
                    quadDefn[outerTetDisc.type - 4][1],
                    quadDefn[outerTetDisc.type - 4][2],
                    quadDefn[outerTetDisc.type - 4][3],
                    4);

                // Set things up to be correct if A,B,C,D == e0,e1,f0,f1.
                triData = discData->data; // First triangle.
                triData->map[triData->nMaps].dest = innerTet[1];
                triData->map[triData->nMaps].vertexMap = Perm<4>(3, 0, 1, 2);
                ++triData->nMaps;
                assert(triData->nMaps <= 2);

                triData = discData->data + 1; // Second triangle.
                triData->map[triData->nMaps].dest = innerTet[0];
                triData->map[triData->nMaps].vertexMap = Perm<4>(3, 0, 1, 2);
                ++triData->nMaps;
                assert(triData->nMaps <= 2);

                // Adjust according to the real arrangement of A,B,C,D.
                adjustQuadMaps(
                    discData->data[0].map[discData->data[0].nMaps - 1],
                    discData->data[1].map[discData->data[1].nMaps - 1],
                    Perm<4>::contract(
                        Perm<5>(e0, e1, f0, f1, f2).inverse() * roles),
                    inner);
            }
        }
    }

    // Run through individual triangles from normal triangles/quadrilaterals,
    // and glue together tetrahedra from the 3-dimensional normal pieces on
    // each side.
    int which;
    for (tet = 0; tet < nTets; ++tet)
        for (type = 0; type < 7; ++type)
            for (pieceNumber = 0; pieceNumber < tetData[tet]->nDiscs(type);
                    ++pieceNumber) {
                discData = &tetData[tet]->data(type, pieceNumber);
                for (which = 0; which < (type < 4 ? 1 : 2); ++which) {
                    triData = discData->data + which;
                    assert(triData->nMaps == 1 || triData->nMaps == 2);
                    if (triData->nMaps > 1) {
                        triData->map[0].dest->join(
                            triData->map[0].vertexMap[3],
                            triData->map[1].dest,
                            triData->map[1].vertexMap *
                                triData->map[0].vertexMap.inverse());
                    }
                }
            }

    // And... we're done!
    // Clean up and go home.
    for (tet = 0; tet < nTets; ++tet)
        delete tetData[tet];
    delete[] tetData;

    inner->intelligentSimplify();
    return inner;
}

} // namespace regina

