
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

#include "algebra/nabeliangroup.h"
#include "dim4/dim4triangulation.h"
#include "maths/matrixops.h"
#include "maths/nmatrixint.h"

namespace regina {

const NAbelianGroup& Dim4Triangulation::getHomologyH1() const {
    if (H1_.known())
        return *H1_.value();

    if (getNumberOfPentachora() == 0)
        return *(H1_ = new NAbelianGroup());

    // Calculate the first homology.
    ensureSkeleton();

    // Build a presentation matrix.
    // Each non-boundary not-in-forest tetrahedron is a generator.
    // Each non-boundary triangle is a relation.
    long nBdryTets = 0;
    long nBdryTriangles = 0;
    for (BoundaryComponentIterator bit = boundaryComponents_.begin();
            bit != boundaryComponents_.end(); ++bit) {
        nBdryTets += (*bit)->countTetrahedra();
        nBdryTriangles += (*bit)->countTriangles();
    }

    // Cast away all unsignedness in case we run into problems subtracting.
    long nGens = static_cast<long>(countTetrahedra()) - nBdryTets
        - static_cast<long>(size())
        + static_cast<long>(countComponents());
    long nRels = static_cast<long>(countTriangles()) - nBdryTriangles;

    NMatrixInt pres(nRels, nGens);

    // Find out which tetrahedron corresponds to which generator.
    long* genIndex = new long[countTetrahedra()];
    long i = 0;
    for (Dim4Tetrahedron* tet : tetrahedra())
        if (! (tet->isBoundary() || tet->inMaximalForest()))
            genIndex[tet->index()] = i++;

    // Run through each triangle and put the corresponding relations into
    // the matrix.
    Dim4Pentachoron* pent;
    int facet;
    Dim4Tetrahedron* tet;
    i = 0;
    for (Dim4Triangle* f : triangles()) {
        if (f->isBoundary())
            continue;

        // Put in the relation corresponding to this triangle.
        for (auto& emb : *f) {
            pent = emb.getPentachoron();
            facet = emb.getVertices()[3];

            tet = pent->tetrahedron(facet);
            if (tet->inMaximalForest())
                continue;

            // We define the "direction" for this dual edge to point
            // from embedding tet->front() to embedding tet->back().
            //
            // Test whether we are traversing this dual edge forwards or
            // backwards as we walk around the triangle (*fit).
            if ((tet->front().getPentachoron() == pent) &&
                    (tet->front().getTetrahedron() == facet))
                pres.entry(i, genIndex[tet->index()]) += 1;
            else
                pres.entry(i, genIndex[tet->index()]) -= 1;
        }

        ++i;
    }

    delete[] genIndex;

    // Build the group from the presentation matrix and tidy up.
    NAbelianGroup* ans = new NAbelianGroup();
    ans->addGroup(pres);
    return *(H1_ = ans);
}

const NAbelianGroup& Dim4Triangulation::getHomologyH2() const {
    if (H2_.known())
        return *H2_.value();

    if (getNumberOfPentachora() == 0)
        return *(H1_ = new NAbelianGroup());

    ensureSkeleton();

    // A placeholder implementation until the heavy machinery is ready.
    // This is correct, but inefficient (uses the full chain maps)
    // and wasteful (since the chain maps will almost certainly end up
    // implemented elsewhere).

    unsigned long i, j;
    unsigned long row, col;
    Dim4Pentachoron* pent;
    Dim4Edge* edge;
    Dim4Triangle* triangle;
    Dim4Tetrahedron* tet;
    NPerm5 perm, tmpPerm;
    int pentEdge, pentTriangle;

    unsigned long nEdges = getNumberOfEdges();
    unsigned long nTriangles = getNumberOfTriangles();
    unsigned long nTetrahedra = getNumberOfTetrahedra();

    // Build a translation table from edge numbers -> "internal edge" indices.
    unsigned long nEdgesInternal = 0;
    unsigned long* edgeInternalIndex = new unsigned long[nEdges];
    for (i = 0; i < nEdges; ++i)
        if (! getEdge(i)->isBoundary())
            edgeInternalIndex[i] = nEdgesInternal++;

    // Build a translation table from triangle numbers -> "internal triangle"
    // indices.
    unsigned long nTrianglesInternal = 0;
    unsigned long* triangleInternalIndex = new unsigned long[nTriangles];
    for (i = 0; i < nTriangles; ++i)
        if (! getTriangle(i)->isBoundary())
            triangleInternalIndex[i] = nTrianglesInternal++;

    // Count the number of internal tetrahedra.
    unsigned long nTetrahedraInternal = 0;
    for (i = 0; i < nTetrahedra; ++i)
        if (! getTetrahedron(i)->isBoundary())
            ++nTetrahedraInternal;

    // --------------------------------------
    // Boundary map from 3-cycles to 2-cycles
    // --------------------------------------
    //
    // Rows: Internal dual polyhedra
    // Cols: Internal dual triangles
    NMatrixInt bdry32(nEdgesInternal, nTrianglesInternal);

    // Build the boundary map, one dual triangle at a time.
    col = 0;
    for (i = 0; i < nTriangles; ++i) {
        triangle = getTriangle(i);
        if (triangle->isBoundary())
            continue;

        // The dual 2-face surrounding this triangle bounds the dual
        // polyhedron surrounding each of its edges.
        pent = triangle->getEmbedding(0).getPentachoron();
        perm = triangle->getEmbedding(0).getVertices();

        for (j = 0; j < 3; ++j) {
            // Edge j of the triangle is opposite vertex j of the triangle.
            edge = triangle->getEdge(j);
            if (edge->isBoundary())
                continue;

            row = edgeInternalIndex[edgeIndex(edge)];
            pentEdge = Dim4Edge::edgeNumber[perm[(j+1) % 3]][perm[(j+2) % 3]];

            tmpPerm = NPerm5(2, j) * perm.inverse() *
                pent->getEdgeMapping(pentEdge);
            // tmpPerm maps (2,3,4) -> (2,3,4), and maps the dual edge into
            // the dual 2-face with the correct orientation.
            // Force (0,1) to map to (0,1), and then read off the sign.
            if (tmpPerm[0] != 0)
                tmpPerm = tmpPerm * NPerm5(0, 1);
            bdry32.entry(row, col) += tmpPerm.sign();
        }

        ++col;
    }

    // --------------------------------------
    // Boundary map from 2-cycles to 1-cycles
    // --------------------------------------
    //
    // Rows: Internal dual 2-faces
    // Cols: Internal dual edges
    NMatrixInt bdry21(nTrianglesInternal, nTetrahedraInternal);

    // Build the boundary map, one dual edge at a time.
    col = 0;
    for (i = 0; i < nTetrahedra; ++i) {
        tet = getTetrahedron(i);
        if (tet->isBoundary())
            continue;

        // The dual edge running through this tetrahedron bounds the dual 2-face
        // surrounding each of its triangles.
        pent = tet->getEmbedding(0).getPentachoron();
        perm = tet->getEmbedding(0).getVertices();

        for (j = 0; j < 4; ++j) {
            // Triangle j of the tetrahedron is opposite vertex j of the
            // tetrahedron.
            triangle = tet->getTriangle(j);
            if (triangle->isBoundary())
                continue;

            row = triangleInternalIndex[triangleIndex(triangle)];
            pentTriangle = Dim4Triangle::triangleNumber
                [perm[(j+1) % 4]][perm[(j+2) % 4]][perm[(j+3) % 4]];
            bdry21.entry(row, col) +=
                (pent->getTriangleMapping(pentTriangle)[4] == perm[4] ? 1 : -1);
        }

        ++col;
    }

    // -------------------------------
    // Finished building boundary maps
    // -------------------------------
    delete[] edgeInternalIndex;
    delete[] triangleInternalIndex;

    // --------------------------------------
    // Pair of boundary maps to abelian group
    // --------------------------------------

    NAbelianGroup* ans = new NAbelianGroup();
    ans->addGroup(bdry32);

    smithNormalForm(bdry21);
    unsigned long lim = (bdry21.rows() < bdry21.columns() ?
        bdry21.rows() : bdry21.columns());

    unsigned long extra = lim;
    for (unsigned long i = 0; i < lim; ++i)
        if (bdry21.entry(i, i) == 0) {
            extra = i;
            break;
        }

    ans->addRank(-extra);
    return *(H2_ = ans);
}

} // namespace regina
