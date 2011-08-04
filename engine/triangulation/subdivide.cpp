
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

#include <vector>

#include "triangulation/ntriangulation.h"
#include "utilities/memutils.h"
#include "utilities/stlutils.h"

namespace regina {

namespace {
    // The indices of the new tetrahedra
    int tetIndex[4][4][4] = {
        { {-1,-1,-1,-1}, {-1,-1,0,1}, {-1,2,-1,3}, {-1,4,5,-1} },
        { {-1,-1,6,7}, {-1,-1,-1,-1}, {8,-1,-1,9}, {10,-1,11,-1} },
        { {-1,12,-1,13}, {14,-1,-1,15}, {-1,-1,-1,-1}, {16,17,-1,-1} },
        { {-1,18,19,-1}, {20,-1,21,-1}, {22,23,-1,-1}, {-1,-1,-1,-1} } };
}

void NTriangulation::barycentricSubdivision() {
    // (Initially written by Dave Letscher on 11/2/00)

    unsigned long nOldTet = tetrahedra.size();
    if (nOldTet == 0)
        return;

    ChangeEventBlock block(this);

    NTriangulation staging;
    NTetrahedron** newTet = new NTetrahedron*[nOldTet * 24];
    NTetrahedron* oldTet;
    NPerm4 p;
    unsigned long tet;
    int face, edge, corner, other;
    for (tet=0; tet<24*nOldTet; tet++)
        newTet[tet] = staging.newTetrahedron();

    // Do all of the internal gluings
    for (tet=0; tet<nOldTet; tet++) {
        for (face=0; face<4; face++)
            for (edge=0; edge<4; edge++)
                if (edge != face)
                    for (corner=0; corner<4; corner++)
                        if ( (face != corner) && (edge != corner) ) {
                            other = 6-face-edge-corner;

                            // Glue to the tetrahedron on the same face and
                            // on the same edge
                            newTet[24*tet+tetIndex[face][edge][corner]]->
                                joinTo(corner,
                                newTet[24*tet+tetIndex[face][edge][other]],
                                NPerm4(corner,other) );

                            // Glue to the tetrahedron on the same face and
                            // at the same corner
                            newTet[24*tet+tetIndex[face][edge][corner]]->
                                joinTo(other,
                                newTet[24*tet+tetIndex[face][other][corner]],
                                NPerm4(edge,other) );

                            // Glue to the tetrahedron on the adjacent face
                            // sharing an edge and a vertex
                            newTet[24*tet+tetIndex[face][edge][corner]]->
                                joinTo(edge,
                                newTet[24*tet+tetIndex[edge][face][corner]],
                                NPerm4(face, edge) );

                            // Glue to the new tetrahedron across an existing
                            // face
                            oldTet = getTetrahedron(tet);
                            if (oldTet->adjacentTetrahedron(face)) {
                                p = oldTet->adjacentGluing(face);
                                newTet[24*tet+tetIndex[face][edge][corner]]->
                                    joinTo(face, newTet[24*tetrahedronIndex(
                                    oldTet->adjacentTetrahedron(face))+
                                    tetIndex[p[face]][p[edge]][p[corner]] ],
                                    NPerm4(p) );
                             }
                        }
    }

    // Delete the existing tetrahedra and put in the new ones.
    removeAllTetrahedra();
    swapContents(staging);
    delete[] newTet;
}

bool NTriangulation::idealToFinite(bool forceDivision) {
    // The call to isValid() ensures the skeleton has been calculated.
    if (isValid() && ! isIdeal())
        if (! forceDivision)
            return false;

    int i,j,k,l;
    int numOldTet = tetrahedra.size();
    if (! numOldTet)
        return false;

    ChangeEventBlock block(this);

    NTriangulation staging;
    NTetrahedron **newTet = new NTetrahedron*[32*numOldTet];
    for (i=0; i<32*numOldTet; i++)
        newTet[i] = staging.newTetrahedron();

    int tip[4];
    int interior[4];
    int edge[4][4];
    int vertex[4][4];

    int nDiv = 0;
    for (j=0; j<4; j++) {
        tip[j] = nDiv++;
        interior[j] = nDiv++;

        for (k=0; k<4; k++)
            if (j != k) {
                edge[j][k] = nDiv++;
                vertex[j][k] = nDiv++;
            }
    }

    // First glue all of the tetrahedra inside the same
    // old tetrahedron together.
    for (i=0; i<numOldTet; i++) {
        // Glue the tip tetrahedra to the others.
        for (j=0; j<4; j++)
            newTet[tip[j] + i * nDiv]->joinTo(j,
                newTet[interior[j] + i * nDiv], NPerm4());

        // Glue the interior tetrahedra to the others.
        for (j=0; j<4; j++) {
            for (k=0; k<4; k++)
                if (j != k) {
                    newTet[interior[j] + i * nDiv]->joinTo(k,
                        newTet[vertex[k][j] + i * nDiv], NPerm4());
                }
        }

        // Glue the edge tetrahedra to the others.
        for (j=0; j<4; j++)
            for (k=0; k<4; k++)
                if (j != k) {
                    newTet[edge[j][k] + i * nDiv]->joinTo(j,
                        newTet[edge[k][j] + i * nDiv], NPerm4(j,k));

                    for (l=0; l<4; l++)
                        if ( (l != j) && (l != k) )
                            newTet[edge[j][k] + i * nDiv]->joinTo(l,
                                newTet[vertex[j][l] + i * nDiv], NPerm4(k,l));
                }
    }

    // Now deal with the gluings between the pieces inside adjacent tetrahedra.
    NTetrahedron *ot;
    int oppTet;
    NPerm4 p;
    for (i=0; i<numOldTet; i++) {
        ot = getTetrahedron(i);
        for (j=0; j<4; j++)
            if (ot->adjacentTetrahedron(j)) {
                 oppTet = tetrahedronIndex(ot->adjacentTetrahedron(j));
                 p = ot->adjacentGluing(j);

                 // First deal with the tip tetrahedra.
                 for (k=0; k<4; k++)
                     if (j != k)
                          newTet[tip[k] + i * nDiv]->joinTo(j,
                              newTet[tip[p[k]] + oppTet * nDiv], p);

                 // Next the edge tetrahedra.
                 for (k=0; k<4; k++)
                     if (j != k)
                         newTet[edge[j][k] + i * nDiv]->joinTo(k,
                             newTet[edge[p[j]][p[k]] + oppTet * nDiv], p);

                 // Finally, the vertex tetrahedra.
                 for (k=0; k<4; k++)
                     if (j != k)
                         newTet[vertex[j][k] + i * nDiv]->joinTo(k,
                             newTet[vertex[p[j]][p[k]] + oppTet * nDiv], p);

            }
    }

    removeAllTetrahedra();
    swapContents(staging);
    calculateSkeleton();

    // Remove the tetrahedra that meet any of the non-standard or
    // ideal vertices.
    // First we make a list of the tetrahedra.
    std::vector<NTetrahedron*> tetList;
    std::vector<NVertexEmbedding>::const_iterator vembit;
    for (VertexIterator vIter = vertices.begin();
            vIter != vertices.end(); vIter++)
        if ((*vIter)->isIdeal() || ! (*vIter)->isStandard())
            for (vembit = (*vIter)->getEmbeddings().begin();
                    vembit != (*vIter)->getEmbeddings().end(); vembit++)
                tetList.push_back((*vembit).getTetrahedron());

    // Now remove the tetrahedra.
    // For each tetrahedron, remove it and delete it.
    for_each(tetList.begin(), tetList.end(),
        std::bind1st(std::mem_fun(&NTriangulation::removeTetrahedron), this));

    return true;
}

bool NTriangulation::finiteToIdeal() {
    if (! hasBoundaryFaces())
        return false;

    // Make a list of all boundary faces, indexed by face number,
    // and create the corresponding new tetrahedra.
    unsigned nFaces = getNumberOfFaces();

    NTriangulation staging;
    NTetrahedron** bdry = new NTetrahedron*[nFaces];
    NPerm4* bdryPerm = new NPerm4[nFaces];
    NTetrahedron** newTet = new NTetrahedron*[nFaces];

    FaceIterator fit;
    unsigned i;
    for (i = 0, fit = faces.begin(); fit != faces.end(); ++i, ++fit) {
        if (! (*fit)->isBoundary()) {
            bdry[i] = newTet[i] = 0;
            continue;
        }

        bdry[i] = (*fit)->getEmbedding(0).getTetrahedron();
        bdryPerm[i] = (*fit)->getEmbedding(0).getVertices();
        newTet[i] = staging.newTetrahedron();
    }

    // Glue the new tetrahedra to each other.
    NEdge* edge;
    unsigned tetFace1, tetFace2;
    NPerm4 t1Perm, t2Perm;
    for (BoundaryComponentIterator bit = boundaryComponents.begin();
            bit != boundaryComponents.end(); bit++)
        for (i = 0; i < (*bit)->getNumberOfEdges(); i++) {
            edge = (*bit)->getEdge(i);

            // This must be a valid boundary edge.
            // Find the boundary faces at either end.
            NEdgeEmbedding e1 = edge->getEmbeddings().front();
            NEdgeEmbedding e2 = edge->getEmbeddings().back();

            tetFace1 = e1.getTetrahedron()->getFace(
                e1.getVertices()[3])->markedIndex();
            tetFace2 = e2.getTetrahedron()->getFace(
                e2.getVertices()[2])->markedIndex();

            t1Perm = bdryPerm[tetFace1].inverse() * e1.getVertices();
            t2Perm = bdryPerm[tetFace2].inverse() * e2.getVertices() *
                NPerm4(2, 3);

            newTet[tetFace1]->joinTo(t1Perm[2], newTet[tetFace2],
                t2Perm * t1Perm.inverse());
        }

    // Now join the new tetrahedra to the boundary faces of the original
    // triangulation.

    // Set up a change block, since here we start changing the original
    // triangulation.
    ChangeEventBlock block(this);

    staging.moveContentsTo(*this);

    for (i = 0; i < nFaces; ++i)
        if (newTet[i])
            newTet[i]->joinTo(3, bdry[i], bdryPerm[i]);

    // Clean up and return.
    delete[] newTet;
    delete[] bdryPerm;
    delete[] bdry;
    return true;
}

} // namespace regina
