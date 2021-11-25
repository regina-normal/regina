
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

#include <set>
#include <vector>

#include "triangulation/dim3.h"

namespace regina {

bool Triangulation<3>::idealToFinite() {
    // The call to isValid() ensures the skeleton has been calculated.
    if (isValid() && ! isIdeal())
        return false;

    int i,j,k,l;
    long numOldTet = simplices_.size();
    if (! numOldTet)
        return false;

    Triangulation<3> staging;
    // Ensure only one event pair is fired in this sequence of changes.
    ChangeEventSpan span1(staging);

    auto* newTet = new Tetrahedron<3>*[32*numOldTet];
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
            newTet[tip[j] + i * nDiv]->join(j,
                newTet[interior[j] + i * nDiv], Perm<4>());

        // Glue the interior tetrahedra to the others.
        for (j=0; j<4; j++) {
            for (k=0; k<4; k++)
                if (j != k) {
                    newTet[interior[j] + i * nDiv]->join(k,
                        newTet[vertex[k][j] + i * nDiv], Perm<4>());
                }
        }

        // Glue the edge tetrahedra to the others.
        for (j=0; j<4; j++)
            for (k=0; k<4; k++)
                if (j != k) {
                    newTet[edge[j][k] + i * nDiv]->join(j,
                        newTet[edge[k][j] + i * nDiv], Perm<4>(j,k));

                    for (l=0; l<4; l++)
                        if ( (l != j) && (l != k) )
                            newTet[edge[j][k] + i * nDiv]->join(l,
                                newTet[vertex[j][l] + i * nDiv], Perm<4>(k,l));
                }
    }

    // Now deal with the gluings between the pieces inside adjacent tetrahedra.
    Tetrahedron<3> *ot;
    long oppTet;
    Perm<4> p;
    for (i=0; i<numOldTet; i++) {
        ot = tetrahedron(i);
        for (j=0; j<4; j++)
            if (ot->adjacentTetrahedron(j)) {
                 oppTet = ot->adjacentTetrahedron(j)->index();
                 p = ot->adjacentGluing(j);

                 // First deal with the tip tetrahedra.
                 for (k=0; k<4; k++)
                     if (j != k)
                          newTet[tip[k] + i * nDiv]->join(j,
                              newTet[tip[p[k]] + oppTet * nDiv], p);

                 // Next the edge tetrahedra.
                 for (k=0; k<4; k++)
                     if (j != k)
                         newTet[edge[j][k] + i * nDiv]->join(k,
                             newTet[edge[p[j]][p[k]] + oppTet * nDiv], p);

                 // Finally, the vertex tetrahedra.
                 for (k=0; k<4; k++)
                     if (j != k)
                         newTet[vertex[j][k] + i * nDiv]->join(k,
                             newTet[vertex[p[j]][p[k]] + oppTet * nDiv], p);

            }
    }

    // Ensure only one event pair is fired in this sequence of changes.
    ChangeEventSpan span2(*this);

    swap(staging);
    ensureSkeleton();

    // Remove the tetrahedra that meet any of the ideal or invalid vertices.
    // First we make a list of the tetrahedra.
    std::vector<Tetrahedron<3>*> tetList;
    for (Vertex<3>* v : vertices())
        if (v->isIdeal() || ! v->isValid())
            for (auto& emb : *v)
                tetList.push_back(emb.tetrahedron());

    // Now remove the tetrahedra.
    // Note: removeTetrahedron() automatically deletes the tetrahedron also.
    for (auto t : tetList)
        removeTetrahedron(t);

    delete[] newTet;
    return true;
}

void Triangulation<3>::puncture(Tetrahedron<3>* tet) {
    if (! tet) {
        // Preconditions disallow empty triangulations, but anyway:
        if (simplices_.empty())
            return;

        tet = simplices_.front();
    }

    // Ensure only one event pair is fired in this sequence of changes.
    ChangeEventSpan span(*this);

    // We will attach a pair of triangular prisms to face 123 of tet.
    // We will join the rectangular walls of the prisms together, and
    // one triangular end from each will join to form the new S^2 boundary.
    Tetrahedron<3>* prism[2][3];

    // Create the new tetrahedra in an order that ensures that the new
    // S^2 boundary will appear in the final two tetrahedra.
    int i, j;
    for (j = 0; j < 3; ++j)
        for (i = 0; i < 2; ++i)
            prism[i][j] = newTetrahedron();

    prism[0][0]->join(0, prism[0][1], Perm<4>(3,0,1,2));
    prism[0][1]->join(0, prism[0][2], Perm<4>(3,0,1,2));

    prism[1][0]->join(1, prism[1][1], Perm<4>(3,0,1,2));
    prism[1][1]->join(1, prism[1][2], Perm<4>(3,2,0,1));

    prism[0][0]->join(1, prism[1][0], Perm<4>(1,2,3,0));
    prism[0][0]->join(2, prism[1][0], Perm<4>(1,2,3,0));
    prism[0][1]->join(1, prism[1][1], Perm<4>(1,2,3,0));
    prism[0][1]->join(2, prism[1][1], Perm<4>(1,2,3,0));
    prism[0][2]->join(1, prism[1][2], Perm<4>(0,1,3,2));
    prism[0][2]->join(2, prism[1][2], Perm<4>(0,1,3,2));

    Tetrahedron<3>* adj = tet->adjacentTetrahedron(0);
    if (adj) {
        Perm<4> gluing = tet->adjacentGluing(0);
        tet->unjoin(0);
        prism[1][0]->join(0, adj, gluing);
    }

    tet->join(0, prism[0][0], Perm<4>(3,0,1,2));
}

void Triangulation<3>::connectedSumWith(const Triangulation<3>& other) {
    if (other.simplices_.empty())
        return;
    if (simplices_.empty()) {
        insertTriangulation(other);
        return;
    }

    // From here we can assume that each triangulation contains at least
    // one tetrahedron.

    // Ensure only one event pair is fired in this sequence of changes.
    ChangeEventSpan span(*this);

    // Insert the other triangulation *before* puncturing this, so that
    // things work in the case where we sum a triangulation with itself.
    unsigned long n = simplices_.size();
    insertTriangulation(other);

    // Make the puncture and record the resulting new boundary triangles.
    puncture(simplices_.front());
    Tetrahedron<3>* bdry[2] = {
        simplices_[simplices_.size() - 2],
        simplices_[simplices_.size() - 1]
    };

    // Pop open a triangle in the second triangulation, and join the
    // two resulting boundary triangles to the boundary from the puncture.
    //
    // Even if the triangle we picked is a boundary triangle (i.e., has
    // degree 1, not degree 2), the overall effect remains correct.

    Triangle<3>* open = simplices_[n]->triangle(0);
    if (open->degree() == 2) {
        const FaceEmbedding<3, 2> emb1 = open->front();
        const FaceEmbedding<3, 2> emb2 = open->back();

        emb1.tetrahedron()->unjoin(emb1.vertices()[3]);

        // We choose the gluing permutations so that, if both triangulations
        // are oriented, the connected sum respects this orientation.
        bool even = (emb1.vertices().sign() > 0);

        if (even) {
            bdry[0]->join(0, emb1.tetrahedron(),
                emb1.vertices() * Perm<4>(3,0,1,2));
            bdry[1]->join(0, emb2.tetrahedron(),
                emb2.vertices() * Perm<4>(3,0,2,1));
        } else {
            bdry[0]->join(0, emb1.tetrahedron(),
                emb1.vertices() * Perm<4>(3,0,2,1));
            bdry[1]->join(0, emb2.tetrahedron(),
                emb2.vertices() * Perm<4>(3,0,1,2));
        }
    } else {
        const FaceEmbedding<3, 2> emb1 = open->front();

        // We choose the gluing permutations so that, if both triangulations
        // are oriented, the connected sum respects this orientation.
        bool even = (emb1.vertices().sign() > 0);

        if (even) {
            bdry[0]->join(0, emb1.tetrahedron(),
                emb1.vertices() * Perm<4>(3,0,1,2));
        } else {
            bdry[0]->join(0, emb1.tetrahedron(),
                emb1.vertices() * Perm<4>(3,0,2,1));
        }
    }
}

} // namespace regina
