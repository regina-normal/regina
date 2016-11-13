
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

#include "algebra/abeliangroup.h"
#include "maths/matrixops.h"
#include "maths/matrix.h"
#include "triangulation/dim4.h"

namespace regina {

const AbelianGroup& Triangulation<4>::homologyH2() const {
    if (H2_.known())
        return *H2_.value();

    if (isEmpty())
        return *(H2_ = new AbelianGroup());

    ensureSkeleton();

    // A placeholder implementation until the heavy machinery is ready.
    // This is correct, but inefficient (uses the full chain maps)
    // and wasteful (since the chain maps will almost certainly end up
    // implemented elsewhere).

    unsigned long i, j;
    unsigned long row, col;
    Pentachoron<4>* pent;
    Edge<4>* e;
    Triangle<4>* t;
    Tetrahedron<4>* tet;
    Perm<5> perm, tmpPerm;
    int pentEdge, pentTriangle;

    unsigned long nEdges = countEdges();
    unsigned long nTriangles = countTriangles();
    unsigned long nTetrahedra = countTetrahedra();

    // Build a translation table from edge numbers -> "internal edge" indices.
    unsigned long nEdgesInternal = 0;
    unsigned long* edgeInternalIndex = new unsigned long[nEdges];
    for (i = 0; i < nEdges; ++i)
        if (! edge(i)->isBoundary())
            edgeInternalIndex[i] = nEdgesInternal++;

    // Build a translation table from triangle numbers -> "internal triangle"
    // indices.
    unsigned long nTrianglesInternal = 0;
    unsigned long* triangleInternalIndex = new unsigned long[nTriangles];
    for (i = 0; i < nTriangles; ++i)
        if (! triangle(i)->isBoundary())
            triangleInternalIndex[i] = nTrianglesInternal++;

    // Count the number of internal tetrahedra.
    unsigned long nTetrahedraInternal = 0;
    for (i = 0; i < nTetrahedra; ++i)
        if (! tetrahedron(i)->isBoundary())
            ++nTetrahedraInternal;

    // --------------------------------------
    // Boundary map from 3-cycles to 2-cycles
    // --------------------------------------
    //
    // Rows: Internal dual polyhedra
    // Cols: Internal dual triangles
    MatrixInt bdry32(nEdgesInternal, nTrianglesInternal);

    // Build the boundary map, one dual triangle at a time.
    col = 0;
    for (i = 0; i < nTriangles; ++i) {
        t = triangle(i);
        if (t->isBoundary())
            continue;

        // The dual 2-face surrounding this triangle bounds the dual
        // polyhedron surrounding each of its edges.
        pent = t->front().pentachoron();
        perm = t->front().vertices();

        for (j = 0; j < 3; ++j) {
            // Edge j of the triangle is opposite vertex j of the triangle.
            e = t->edge(j);
            if (e->isBoundary())
                continue;

            row = edgeInternalIndex[e->index()];
            pentEdge = Edge<4>::edgeNumber[perm[(j+1) % 3]][perm[(j+2) % 3]];

            tmpPerm = Perm<5>(2, j) * perm.inverse() *
                pent->edgeMapping(pentEdge);
            // tmpPerm maps (2,3,4) -> (2,3,4), and maps the dual edge into
            // the dual 2-face with the correct orientation.
            // Force (0,1) to map to (0,1), and then read off the sign.
            if (tmpPerm[0] != 0)
                tmpPerm = tmpPerm * Perm<5>(0, 1);
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
    MatrixInt bdry21(nTrianglesInternal, nTetrahedraInternal);

    // Build the boundary map, one dual edge at a time.
    col = 0;
    for (i = 0; i < nTetrahedra; ++i) {
        tet = tetrahedron(i);
        if (tet->isBoundary())
            continue;

        // The dual edge running through this tetrahedron bounds the dual 2-face
        // surrounding each of its triangles.
        pent = tet->front().pentachoron();
        perm = tet->front().vertices();

        for (j = 0; j < 4; ++j) {
            // Triangle j of the tetrahedron is opposite vertex j of the
            // tetrahedron.
            t = tet->triangle(j);
            if (t->isBoundary())
                continue;

            row = triangleInternalIndex[t->index()];
            pentTriangle = Triangle<4>::triangleNumber
                [perm[(j+1) % 4]][perm[(j+2) % 4]][perm[(j+3) % 4]];
            bdry21.entry(row, col) +=
                (pent->triangleMapping(pentTriangle)[4] == perm[4] ? 1 : -1);
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

    AbelianGroup* ans = new AbelianGroup();
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
